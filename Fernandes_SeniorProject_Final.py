#Moises Fernandes 
#Senior Design Project 
#Computer Engineering 
#Autonomous Water Sensing Boat 

import serial
import mysql.connector

def read_serial_data(port, baud_rate):
    # The function to read data from the serial port remains the same.
    # Please keep this function as it is.

# Baud rate and Bluetooth serial port name
baud_rate = 38400
port_name = 'COM5'   

def setup_serial_connection(port_name, baud_rate):
    try:
        # Create a serial connection
        ser = serial.Serial(port_name, baud_rate)
        return ser
    except serial.SerialException as e:
        print(f"Serial Exception: {e}")
        return None

def close_serial_connection(ser):
    # Close the serial connection
    if ser is not None and ser.is_open:
        ser.close()

def edit_water_sample_data():
    # The function to edit water sample data remains the same.
    # Please keep this function as it is.

def sensor_data_insert(arduino_file_path, db_config):
    try:
        # Read sensor data points from the Arduino file
        with open(arduino_file_path, 'r') as arduino_file:
            sensor_data_points = arduino_file.readlines()

        # Process and convert the data to a suitable format
        # Assuming the Arduino file contains one sensor data point per line in the format "sensor_value"
        processed_data = [float(data.strip()) for data in sensor_data_points]

        # Establish a connection to the SQL database
        connection = mysql.connector.connect(**db_config)
        cursor = connection.cursor()

        # Create a table in the database to store the sensor data
        create_table_query = "CREATE TABLE IF NOT EXISTS sensor_data (id INT AUTO_INCREMENT PRIMARY KEY, value FLOAT)"
        cursor.execute(create_table_query)

        # Insert the sensor data into the SQL database table
        insert_query = "INSERT INTO sensor_data (value) VALUES (%s)"
        data_to_insert = [(value,) for value in processed_data]
        cursor.executemany(insert_query, data_to_insert)

        # Commit changes and close the connection
        connection.commit()
        cursor.close()
        connection.close()

        print("Sensor data successfully inserted into the SQL database.")

    except FileNotFoundError:
        print("Arduino file not found.")
    except mysql.connector.Error as e:
        print(f"Error while inserting data to SQL: {e}")
    except Exception as ex:
        print(f"Unexpected error occurred: {ex}")

if __name__ == "__main__":
    port_name = 'COM5'  # Bluetooth serial port name
    ser = setup_serial_connection(port_name, baud_rate)

    if ser:
        try:
            # Read data from the serial port
            while True:
                data = ser.readline().decode('utf-8').strip()
                print("Received:", data)
        except KeyboardInterrupt:
            print("Exiting...")
        finally:
            close_serial_connection(ser)

    # Call the function to edit water sample data
    edit_water_sample_data()

    # Call the function to insert sensor data
    arduino_file_path = "path/to/arduino_file.txt"  # Replace with the actual path to your Arduino file
    db_config = {
        'host': 'localhost',
        'user': 'your_username',
        'password': 'your_password',
        'database': 'your_database_name'
    }
    sensor_data_insert(arduino_file_path, db_config)
